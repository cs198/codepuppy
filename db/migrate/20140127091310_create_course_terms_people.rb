class CreateCourseTermStudents < ActiveRecord::Migration
  def change
    create_table :course_terms_people do |t|
      t.belongs_to :course_term
      t.belongs_to :person
    end

    add_index :course_terms_people, :course_term_id
    add_index :course_terms_people, :person_id
  end
end
