# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20140127090813) do

  # These are extensions that must be enabled in order to support this database
  enable_extension "plpgsql"

  create_table "course_terms", force: true do |t|
    t.string  "course_name"
    t.string  "term_name"
    t.string  "period"
    t.boolean "active"
    t.integer "course_id"
  end

  add_index "course_terms", ["course_id"], name: "index_course_terms_on_course_id", using: :btree

  create_table "courses", force: true do |t|
    t.string "course_system_id"
  end

  add_index "courses", ["course_system_id"], name: "index_courses_on_course_system_id", unique: true, using: :btree

  create_table "people", force: true do |t|
    t.string   "user_system_id"
    t.string   "given_name"
    t.string   "family_name"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "people", ["user_system_id"], name: "index_people_on_user_system_id", unique: true, using: :btree

end
