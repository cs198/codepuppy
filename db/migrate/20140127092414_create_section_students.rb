class CreateSectionStudents < ActiveRecord::Migration
  def change
    create_table :section_students do |t|
    end

    add_reference :section_students, :student, index: true
    add_reference :section_students, :section, index: true
  end
end
