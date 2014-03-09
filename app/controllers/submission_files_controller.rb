class SubmissionFilesController < ApplicationController
  skip_before_filter :verify_authenticity_token
  respond_to :html, :xml, :json

  def index
    submission_file = SubmissionFile.find_all_by_submission_id(
      params[:submission_id]
    )
    respond_with(submission_file)
  rescue ActiveRecord::RecordNotFound
    raise 'Submission file not found'
  end

  def create
    submission_file = SubmissionFile.create(submission_file_params)
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

  def comments
    comments = FileComment.find_all_by_submission_file_id(
      params[:submission_file_id])
    respond_with(comments)
  rescue ActiveRecord::RecordNotFound
    raise 'Submission file\'s comments not found'
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
