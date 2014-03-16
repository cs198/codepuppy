class SubmissionFilesController < ApiController
  skip_before_filter :verify_authenticity_token
  respond_to :json

  def index
    submission_file = SubmissionFile.find_all_by_submission_id(
      params[:submission_id]
    )
    respond_with(submission_file)
  rescue ActiveRecord::RecordNotFound
    raise 'Submission file not found'
  end

  def create
    file = params[:file]
    submission_file = SubmissionFile.create(
      submission_id: params[:submission_id],
      original_filename: file.original_filename,
      data: file.read
    )
    respond_with(submission_file)
  rescue ActiveRecord::RecordInvalid => invalid
    puts invalid.record.errors
  end

  def show
    submission_file = SubmissionFile.find(params[:id])
    respond_with(submission_file)
  rescue ActiveRecord::RecordNotFound
    raise 'Submission file not found'
  end

  private

  def submission_file_params
    params.permit(
      :submission_id,
      :original_filename,
      :data
    )
  end
end
