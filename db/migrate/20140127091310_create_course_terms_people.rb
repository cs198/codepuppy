class CreateCourseTermsPeople < ActiveRecord::Migration
  def change
    create_table :course_terms_people do |t|
      t.belongs_to :course_term, index: true
      t.belongs_to :person, index: true
      t.string :role
    end
  end
end
