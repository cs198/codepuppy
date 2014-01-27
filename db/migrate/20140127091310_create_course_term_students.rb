class CreateCourseTermStudents < ActiveRecord::Migration
  def change
    create_table :course_term_students do |t|
    end
    add_reference :course_term_students, :course_term, index: true
    add_reference :course_term_students, :person, index: true
  end
end
