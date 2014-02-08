class SubmissionsController < ApplicationController

  def create
    Submission.create(submission_params)
  end

  private 

    def submission_params
      params.require(:submission).permit(:student_id, :assignment_id, :location, :feedback_released)
    end

end
