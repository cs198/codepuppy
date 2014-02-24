class PeopleController < ApplicationController
  respond_to :html, :xml, :json

  def create
    Person.create(person_params)
  end

  def show
    person = Person.find(params[:id])
    respond_with(person)
  rescue ActiveRecord::RecordNotFound
    raise 'Person not found'
  end

  def course_terms_taking
    student = Person.find(params[:id])
    respond_with(student.course_terms_taking)
  rescue ActiveRecord::RecordNotFound
    raise 'Person not found'
  end

  def course_terms_administrating
    admin = Person.find(params[:id])
    respond_with(admin.course_terms_administrating)
  rescue ActiveRecord::RecordNotFound
    raise 'Person not found'
  end

  def course_terms_leading
    leader = Person.find(params[:id])
    respond_with(leader.course_terms_leading)
  rescue ActiveRecord::RecordNotFound
    raise 'Person not found'
  end

  private

  def person_params
    params.require(:person).permit(:user_system_id, :given_name, :family_name)
  end
end
