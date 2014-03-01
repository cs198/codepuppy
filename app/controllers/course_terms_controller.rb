class CourseTermsController < ApplicationController
  skip_before_filter :verify_authenticity_token
  respond_to :html, :xml, :json

  def create
    course_term = CourseTerm.create(course_terms_params)
    respond_with(course_term)
  rescue ActiveRecord::RecordInvalid => invalid
    puts invalid.record.errors
  end

  def show
    course_term = CourseTerm.find(params[:id])
    respond_with(course_term)
  rescue ActiveRecord::RecordNotFound
    raise 'Course term not found'
  end

  def active
    respond_with(CourseTerm.where(:active => true))
  rescue ActiveRecord::RecordNotFound
    raise 'Course term not found'
  end

  def students
    course_term = CourseTerm.find(params[:id])
    respond_with(course_term.students)
  rescue ActiveRecord::RecordNotFound
    raise 'Course term not found'
  end

  def leaders
    course_term = CourseTerm.find(params[:id])
    respond_with(course_term.leaders)
  rescue ActiveRecord::RecordNotFound
    raise 'Course term not found'
  end

  def admins
    course_term = CourseTerm.find(params[:id])
    respond_with(course_term.admins)
  rescue ActiveRecord::RecordNotFound
    raise 'Course term not found'
  end

  private

  def course_terms_params
    params.permit(
      :course_id,
      :course_name,
      :term_name,
      :period,
      :active
    )
  end
end
