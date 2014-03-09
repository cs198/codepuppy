class SubmissionsController < ApplicationController
  skip_before_filter :verify_authenticity_token

  respond_to :html, :xml, :json

  def index
    submissions = Submission.find_all_by_assignment_id(params[:assignment_id])
    person_submissions = []
    submissions.each do |submission|
      student = Person.find(submission.student_id)
      person_submissions.push('student' => student, 'submission' => submission)
    end
    respond_with(person_submissions)
  rescue ActiveRecord::RecordNotFound
    raise 'Assignment not found'
  end

  def create
    submission = Submission.create(submission_params)
    respond_with(submission)
   rescue ActiveRecord::RecordInvalid => invalid
    puts invalid.record.errors
  end

  def show
    submission = Submission.find(params[:id])
    student = Person.find(submission.student_id)
    person_submission = { 'student' => student, 'submission' => submission }
    respond_with(person_submission)
  rescue ActiveRecord::RecordNotFound
    raise 'Submission not found'
  end

  def files
    files = SubmissionFile.find_all_by_submission_id(params[:submission_id])
    respond_with(files)
  rescue ActiveRecord::RecordNotFound
    raise 'Submission not found'
  end

  private

  def submission_params
    params.permit(:student_id, :assignment_id, :feedback_released)
  end
end
