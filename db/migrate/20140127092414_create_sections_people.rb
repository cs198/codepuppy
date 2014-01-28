class CreateSectionsPeople < ActiveRecord::Migration
  def change
    create_table :sections_people do |t|
      t.belongs_to :person, index: true
      t.belongs_to :section, index: true
      t.boolean :student
    end
  end
end
