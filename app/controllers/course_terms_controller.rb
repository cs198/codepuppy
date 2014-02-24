class CourseTermsController < ApplicationController
  respond_to :html, :xml, :json

  def create
    CourseTerm.create(course_terms_params)
  end

  def show
    course_term = CourseTerm.find(params[:id])
    respond_with(course_term)
  rescue ActiveRecord::RecordNotFound
    raise 'Course term not found'
  end

  def active
    course_terms = []
    Course.all.each do |course|
      course_terms.concat course.active
    end
    respond_with(course_terms)
  rescue ActiveRecord::RecordNotFound
    raise 'Course term not found'
  end

  def assignments
    assignments = Assignment.find_all_by_course_term_id(
      params[:course_term_id]
    )
    respond_with(assignments)
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
    params.require(:course_term).permit(
      :course_id,
      :course_name,
      :term_name,
      :period,
      :active
    )
  end
end
