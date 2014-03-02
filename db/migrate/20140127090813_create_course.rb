class CreateCourse < ActiveRecord::Migration
  def change
    create_table :courses do |t|
      t.string :course_dept
      t.string :course_code
      t.string :course_name
      t.string :term_name
      t.string :period
      t.boolean :active
    end
  end
end
