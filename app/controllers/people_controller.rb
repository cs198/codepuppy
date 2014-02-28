class PeopleController < ApplicationController
  skip_before_filter :verify_authenticity_token
  respond_to :html, :xml, :json

  def create
    person = Person.create(person_params)
    respond_with(person)
  rescue ActiveRecord::RecordInvalid => invalid
    puts invalid.record.errors
  end

  def show
    person = Person.find(params[:id])
    respond_with(person)
  rescue ActiveRecord::RecordNotFound
    raise 'Person not found'
  end

  def course_terms
    person = Person.find(params[:person_id])
    if !params.key?(:role)
      respond_with(course_terms_builder_all(person))
    else
      respond_with(course_terms_builder(person, params[:role]))
    end
  rescue ActiveRecord::RecordNotFound
    raise 'Person not found'
  end

  private

  def person_params
    params.permit(:user_system_id, :given_name, :family_name)
  end

  def course_terms_builder_all(person)
    course_terms_with_roles =
      course_terms_builder(person, 'student') +
      course_terms_builder(person, 'leader') +
      course_terms_builder(person, 'admin')
    course_terms_with_roles
  end

  def course_terms_builder(person, role)
    course_terms_with_roles = []
    course_terms_without_roles(person, role).each do |course_term|
      course_terms_with_roles.push(
        'course_term' => course_term, 'role' => role
      )
    end
    course_terms_with_roles
  end

  def course_terms_without_roles(person, role)
    course_terms_without_roles = []
    if role.eql?('student')
      course_terms_without_roles = person.course_terms.taking
    elsif role.eql?('leader')
      course_terms_without_roles = person.course_terms.leading
    elsif role.eql?('admin')
      course_terms_without_roles = person.course_terms.administrating
    else
      fail "Role requested does not exist. Inputted: #{role.to_s}; " +
           'Valid roles: student, leader, admin.'
    end
    course_terms_without_roles
  end
end
