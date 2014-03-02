class SectionsController < ApplicationController
  skip_before_filter :verify_authenticity_token
  respond_to :html, :xml, :json

  def create
    section = Section.create(section_params)
    respond_with(section)
  rescue ActiveRecord::RecordInvalid => invalid
    puts invalid.record.errors
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
    params.permit(:course_id, :leader_id)
  end
end
