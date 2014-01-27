class CreateCourseTermAdmins < ActiveRecord::Migration
  def change
    create_table :course_term_admins do |t|
    end

    add_reference :course_term_admins, :course_term, index: true
    add_reference :course_term_admins, :person, index: true
  end
end
