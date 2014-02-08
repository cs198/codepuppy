class AssignmentsController < ApplicationController

  def create
    Assignment.create(assignment_params)
  end

  private

    def assignment_params
      params.require(:assignment).permit(:course_term_id, :number, :date_assigned, :date_due)
    end

end
