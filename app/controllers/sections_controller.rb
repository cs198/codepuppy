class SectionsController < ApplicationController
  respond_to :html, :xml, :json

  def create
    Section.create(section_params)
  end

  def show
    section = Section.find(params[:id])
    respond_with(section)
  rescue ActiveRecord::RecordNotFound
    raise 'Section not found'
  end

  def leaders
    section = Section.find(params[:id])
    respond_with(section.leaders)
  rescue ActiveRecord::RecordNotFound
    raise 'Section not found'
  end

  def students
    section = Section.find(params[:id])
    respond_with(section.students)
  rescue ActiveRecord::RecordNotFound
    raise 'Section not found'
  end

  private

  def section_params
    params.require(:section).permit(:course_term_id, :leader_id)
  end
end
