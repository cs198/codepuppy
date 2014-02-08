class CourseTermsController < ApplicationController

  def create
    CourseTerm.create(course_terms_params)
  end

  private

    def course_terms_params
      params.require(:course_term).permit(:course_id, :course_name, :term_name, :period, :active)
    end

end
