class CoursesController < ApplicationController
  skip_before_filter :verify_authenticity_token
  respond_to :html, :xml, :json

  def create
    course = Course.create(courses_params)
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

  def active
    respond_with(Course.where(active: true))
  rescue ActiveRecord::RecordNotFound
    raise 'Course not found'
  end

  def students
    course = Course.find(params[:course_id])
    respond_with(course.people.students)
  end

  def leaders
    course = Course.find(params[:course_id])
    respond_with(course.people.leaders)
  rescue ActiveRecord::RecordNotFound
    raise 'Course not found'
  end

  def admins
    course = Course.find(params[:course_id])
    respond_with(course.people.admins)
  rescue ActiveRecord::RecordNotFound
    raise 'Course not found'
  end

  private

  def courses_params
    params.permit(
      :course_dept,
      :course_code,
      :course_name,
      :term_name,
      :period,
      :active
    )
  end
end
