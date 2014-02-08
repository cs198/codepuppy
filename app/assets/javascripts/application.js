// This is a manifest file that'll be compiled into application.js, which will include all the files
// listed below.
//
// Any JavaScript/Coffee file within this directory, lib/assets/javascripts, vendor/assets/javascripts,
// or vendor/assets/javascripts of plugins, if any, can be referenced here using a relative path.
//
// It's not advisable to add code directly here, but if you do, it'll appear at the bottom of the
// compiled file.
//
// Read Sprockets README (https://github.com/sstephenson/sprockets#sprockets-directives) for details
// about supported directives.
//
//= require_self
//= require_tree .

codepuppy = angular.module('codepuppy', ['ngRoute']);

codepuppy.config(function($routeProvider) {
  //if ($user == 'student') {
  if (true) {
    $routeProvider
    .when('/', {
      templateUrl : '/assets/home.html',
      controller  : 'MainCtrl'
    })
    .when('/:classID', {
      templateUrl : '/assets/class.html',
      controller  : 'ClassCtrl'
    })
    .when('/:classID/:assignmentID', {
      templateUrl : '/assets/assignment.html',
      controller  : 'AssignmentCtrl'
    })

//} else if ($user == 'section leader') {
//     more stuff
//} else if ($user == 'lecturer') {
//     more stuff

  }
});

