class SectionsController < ApplicationController

  def create
    Section.create(section_params)
  end

  private

    def section_params
      params.require(:section).permit(:course_term_id, :leader_id)
    end

end
