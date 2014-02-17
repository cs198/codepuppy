class SubmissionsController < ApplicationController

  respond_to :html, :xml, :json

  def create
    Submission.create(submission_params)
  end

  def show
    begin
      submission = Submission.find(params[:id])
      respond_with(submission)
    rescue ActiveRecord::RecordNotFound
      raise 'Submission not found'
    end
  end

  def comments
    begin
      comments = Comment.find_all_by_submission_id(params[:submission_id])
      respond_with(comments)
    rescue ActiveRecord::RecordNotFound
      raise 'Submission not found'
    end
  end

  private 

    def submission_params
      params.require(:submission).permit(:student_id, :assignment_id, :location, :feedback_released)
    end

end
