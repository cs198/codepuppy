class SectionsController < ApplicationController

  respond_to :html, :xml, :json

  def create
    Section.create(section_params)
  end

  def show
    begin
      section = Section.find(params[:id])
      respond_with(section)
    rescue ActiveRecord::RecordNotFound
      raise 'Section not found'
    end
  end

  def leaders
    begin
      section = Section.find(params[:id])
      respond_with(section.leaders)
    rescue ActiveRecord::RecordNotFound
      raise 'Section not found'
    end
  end

  def students
    begin
      section = Section.find(params[:id])
      respond_with(section.students)
    rescue ActiveRecord::RecordNotFound
      raise 'Section not found'
    end
  end


  private

    def section_params
      params.require(:section).permit(:course_term_id, :leader_id)
    end

end
