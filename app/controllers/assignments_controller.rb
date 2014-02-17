class AssignmentsController < ApplicationController

  respond_to :html, :xml, :json

  def create
    Assignment.create(assignment_params)
  end

  def show
    begin
      assignment = Assignment.find(params[:id])
      respond_with(assignment)
    rescue ActiveRecord::RecordNotFound
      raise 'Assignment not found'
    end
  end

  def submissions
    begin
      submissions = Submission.find_all_by_assignment_id(params[:assignment_id])
      respond_with(submissions)
    rescue ActiveRecord::RecordNotFound
      raise 'Assignment not found'
    end
  end

  private

    def assignment_params
      params.require(:assignment).permit(:course_term_id, :number, :date_assigned, :date_due)
    end

end
