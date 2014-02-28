class AssignmentsController < ApplicationController
  skip_before_filter :verify_authenticity_token
  respond_to :html, :xml, :json

  def create
    assignment = Assignment.create(assignment_params)
    respond_with(assignment)
  rescue ActiveRecord::RecordInvalid => invalid
    puts invalid.record.errors
  end

  def show
    assignment = Assignment.find(params[:id])
    respond_with(assignment)
  rescue ActiveRecord::RecordNotFound
    raise 'Assignment not found'
  end

  def submissions
    submissions = Submission.find_all_by_assignment_id(params[:assignment_id])
    respond_with(submissions)
  rescue ActiveRecord::RecordNotFound
    raise 'Assignment not found'
  end

  private

  def assignment_params
    params.permit(
      :course_term_id,
      :number,
      :date_assigned,
      :date_due
    )
  end
end
