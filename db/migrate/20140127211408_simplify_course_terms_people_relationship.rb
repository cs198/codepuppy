class SimplifyCourseTermsPeopleRelationship < ActiveRecord::Migration
  def change
    drop_table :course_term_students
    drop_table :course_term_admins
    drop_table :course_term_leaders

    create_table :course_terms_people do |t|
      t.string :role
    end

    add_reference :course_terms_people, :course_term, index: true
    add_reference :course_terms_people, :person, index: true

    add_index :course_terms_people, :role
    add_index :course_terms_people, [:course_term_id, :role]
  end
end
