class CoursesController < ApplicationController

  def create
    Course.create(course_params)
  end

  private

    def course_params
      params.require(:course).permit(:course_system_id)
    end

end
