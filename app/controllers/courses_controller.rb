class CoursesController < ApplicationController
  skip_before_filter :verify_authenticity_token
  respond_to :html, :xml, :json

  def create
    course = Course.create(course_params)
    respond_with(course)
  rescue ActiveRecord::RecordInvalid => invalid
    puts invalid.record.errors
  end

  def show
    course = Course.find(params[:id])
    respond_with(course)
  rescue ActiveRecord::RecordNotFound
    raise 'Course not found'
  end

  def index
    courses = Course.all
    respond_with(courses)
  rescue ActiveRecord::RecordNotFound
    raise 'Courses not found'
  end

  private

  def course_params
    params.permit(:course_system_id)
  end
end
