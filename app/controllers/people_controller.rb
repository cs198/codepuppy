class PeopleController < ApiController
  skip_before_filter :verify_authenticity_token
  respond_to :json

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

  def courses
    person = Person.find(params[:person_id])
    if !params.key?(:role)
      respond_with(courses_builder_all(person))
    else
      respond_with(courses_builder(person, params[:role]))
    end
  rescue ActiveRecord::RecordNotFound
    raise 'Person not found'
  end

  private

  def person_params
    params.permit(:user_system_id, :given_name, :family_name)
  end

  def courses_builder_all(person)
    courses_with_roles =
      courses_builder(person, 'student') +
      courses_builder(person, 'leader') +
      courses_builder(person, 'admin')
    courses_with_roles
  end

  def courses_builder(person, role)
    courses_with_roles = []
    courses_without_roles(person, role).each do |course|
      courses_with_roles.push(
        course: course, role: role
      )
    end
    courses_with_roles
  end

  def courses_without_roles(person, role)
    courses_without_roles = []
    if role.eql?('student')
      courses_without_roles = person.courses.taking
    elsif role.eql?('leader')
      courses_without_roles = person.courses.leading
    elsif role.eql?('admin')
      courses_without_roles = person.courses.administrating
    else
      fail "Role requested does not exist. Inputted: #{role.to_s}; " +
           'Valid roles: student, leader, admin.'
    end
    courses_without_roles
  end
end
