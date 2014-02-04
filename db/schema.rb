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

ActiveRecord::Schema.define(version: 20140127093535) do

  # These are extensions that must be enabled in order to support this database
  enable_extension "plpgsql"

  create_table "assignments", force: true do |t|
    t.integer  "course_term_id"
    t.integer  "number"
    t.datetime "date_assigned"
    t.datetime "date_due"
  end

  add_index "assignments", ["course_term_id"], name: "index_assignments_on_course_term_id", using: :btree

  create_table "comments", force: true do |t|
    t.integer "submission_id"
    t.string  "file_location"
    t.integer "line_number"
    t.integer "num_lines"
    t.string  "comment"
  end

  add_index "comments", ["file_location"], name: "index_comments_on_file_location", using: :btree
  add_index "comments", ["submission_id"], name: "index_comments_on_submission_id", using: :btree

  create_table "course_terms", force: true do |t|
    t.integer "course_id"
    t.string  "course_name"
    t.string  "term_name"
    t.string  "period"
    t.boolean "active"
  end

  add_index "course_terms", ["course_id"], name: "index_course_terms_on_course_id", using: :btree

  create_table "course_terms_people", force: true do |t|
    t.integer "course_term_id"
    t.integer "person_id"
    t.string  "role"
  end

  add_index "course_terms_people", ["course_term_id"], name: "index_course_terms_people_on_course_term_id", using: :btree
  add_index "course_terms_people", ["person_id"], name: "index_course_terms_people_on_person_id", using: :btree

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

  create_table "sections", force: true do |t|
    t.integer "course_term_id"
    t.integer "leader_id"
  end

  add_index "sections", ["course_term_id"], name: "index_sections_on_course_term_id", using: :btree
  add_index "sections", ["leader_id"], name: "index_sections_on_leader_id", using: :btree

  create_table "sections_people", force: true do |t|
    t.integer "person_id"
    t.integer "section_id"
    t.string  "role"
  end

  add_index "sections_people", ["person_id"], name: "index_sections_people_on_person_id", using: :btree
  add_index "sections_people", ["section_id"], name: "index_sections_people_on_section_id", using: :btree

  create_table "submissions", force: true do |t|
    t.integer  "student_id"
    t.integer  "assignment_id"
    t.string   "location"
    t.datetime "date_submitted"
    t.boolean  "feedback_released"
  end

  add_index "submissions", ["assignment_id"], name: "index_submissions_on_assignment_id", using: :btree
  add_index "submissions", ["student_id", "assignment_id"], name: "index_submissions_on_student_id_and_assignment_id", using: :btree
  add_index "submissions", ["student_id"], name: "index_submissions_on_student_id", using: :btree

end
