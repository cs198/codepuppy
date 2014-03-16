class SubmissionsController < ApiController
  skip_before_filter :verify_authenticity_token
  respond_to :json

  def index
    begin
      submissions = Submission.find_all_by_assignment_id(params[:assignment_id])
    rescue ActiveRecord::RecordNotFound
      raise "No submissions with assignment id #{params[:assignment_id]} found"
    end

    person_submissions = people_for_submissions(submissions)
    respond_with(person_submissions)
  end

  def create
    submission = Submission.create(submission_params)
    respond_with(submission)
   rescue ActiveRecord::RecordInvalid => invalid
    puts invalid.record.errors
  end

  def show
    submission = Submission.find(params[:id])
    person = Person.find(submission.person_id)
    person_submission = { 'person' => person, 'submission' => submission }
    respond_with(person_submission)
  rescue ActiveRecord::RecordNotFound
    raise 'Submission not found'
  end

  def submission_files
    files = SubmissionFile.find_all_by_submission_id(params[:submission_id])
    respond_with(files)
  rescue ActiveRecord::RecordNotFound
    raise 'Submission not found'
  end

  def update_feedback_released
    if params.key?(:submission_id)
      submission = Submission.find(params[:submission_id])
      submission.update_attribute(:feedback_released, params[:feedback_released])

      # NOTE: Rails is really weird with PUT requests and
      # defaults to sending Code 204: No Content responses
      # upon respond_with calls. To respond with an actual
      # resource, we break from our respond_with paradigm
      # and render json instead. You will have to do this
      # for every PUT request.

      render json: submission
    end
  rescue ActiveRecord::RecordNotFound
    raise 'Submission not found'
  end

  private

  def people_for_submissions(submissions)
    person_submissions = []
    submissions.each do |submission|
      begin
        person = Person.find(submission.person_id)
      rescue ActiveRecord::RecordNotFound
        # TODO: switch back to raise after we fix submission uploads
        # Right now, we use '1' as the ID for an upload, which causes
        # this rescue.
        next
    #    raise "When finding submissions for assignment #{params[:assignment_id]}"\
    #      ", no person with ID #{submission.person_id} found."
      end

      person_submissions.push('person' => person, 'submission' => submission)
    end
    person_submissions
  end

  def submission_params
    params.permit(:person_id, :assignment_id, :feedback_released, :date_submitted)
  end
end
