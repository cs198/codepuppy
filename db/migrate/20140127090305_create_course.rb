class CreateCourse < ActiveRecord::Migration
  def change
    create_table :courses do |t|
      t.string :course_system_id
    end

    add_index :courses, :course_system_id, unique: true
  end
end
