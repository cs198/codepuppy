class AssignmentsController < ApiController
  skip_before_filter :verify_authenticity_token
  respond_to :json

  def index
    assignments = Assignment.find_all_by_course_id(
      params[:course_id]
    )
    respond_with(assignments)
  rescue ActiveRecord::RecordNotFound
    raise 'Course not found'
  end

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

  def update
    assignment = Assignment.find(params[:assignment_id])
    unless assignment.update_attributes(assignment_params)
      puts 'Unable to update assignment ' + assignment.id
    end
    render json: assignment
  rescue ActiveRecord::RecordNotFound
    raise 'Assignment not found'
  end

  def destroy
    assignment = Assignment.find(params[:id])
    assignment.destroy
    render json: []
  rescue ActiveRecord::RecordNotFound
    raise 'Assignment not found'
  end

  private

  def assignment_params
    params.permit(
      :course_id,
      :number,
      :date_assigned,
      :date_due,
      :name,
      :description,
      :pdf_url
    )
  end
end
