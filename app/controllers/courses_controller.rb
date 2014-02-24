class CoursesController < ApplicationController
  respond_to :html, :xml, :json

  def create
    Course.create(course_params)
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
    params.require(:course).permit(:course_system_id)
  end
end
