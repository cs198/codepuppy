class CreateSectionStudents < ActiveRecord::Migration
  def change
    create_table :section_students do |t|
      t.belongs_to :student, index: true
      t.belongs_to :section, index: true
    end
  end
end
