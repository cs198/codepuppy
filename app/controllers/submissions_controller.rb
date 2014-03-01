class SubmissionsController < ApplicationController
  skip_before_filter :verify_authenticity_token

  respond_to :html, :xml, :json

  def index
    submissions = Submission.find_all_by_assignment_id(params[:assignment_id])
    respond_with(submissions)
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
    respond_with(submission)
  rescue ActiveRecord::RecordNotFound
    raise 'Submission not found'
  end

  def comments
    comments = Comment.find_all_by_submission_id(params[:submission_id])
    respond_with(comments)
  rescue ActiveRecord::RecordNotFound
    raise 'Submission not found'
  end

  private

  def submission_params
    params.permit(:student_id, :assignment_id, :location, :feedback_released)
  end
end
