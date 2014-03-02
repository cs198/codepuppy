class CreateCoursesPeople < ActiveRecord::Migration
  def change
    create_table :courses_people do |t|
      t.belongs_to :course, index: true
      t.belongs_to :person, index: true
      t.string :role
    end
  end
end
