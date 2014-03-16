//= require_self
//= require_tree .

codepuppy = angular.module('codepuppy', ['ngRoute', 'ngCookies', 'ui.bootstrap',
  'angularFileUpload']);

codepuppy.config(function($routeProvider) {
  // TODO: use session information to determine role.  Should be one of
  // 'student', 'leader'
  var upperCaseFirst = function(str) {
    if (str.length === 0) {
        return str;
    }
    return str.charAt(0).toUpperCase() + str.slice(1);
  };

  var role = 'student';
  var capRole = upperCaseFirst(role);

  // Student-specific routes:
  if (role === 'student') {
    $routeProvider.when('/courses/:courseID/assignments/:assignmentID/submit', {
      templateUrl: '/assets/pages/fileUpload/' + role + '/upload.html',
      controller: 'FileUploadCtrl'
    });
  }

  $routeProvider
  .when('/', {
    templateUrl: '/assets/pages/home/' + role + '/' + role + 'Home.html',
    controller: capRole + 'HomeCtrl'
  })
  .when('/courses/:courseID', {
    templateUrl: '/assets/pages/course/' + role + '/' + role +
        'Course.html',
    controller: capRole + 'CourseCtrl'
  })
  .when('/courses/:courseID/assignments/:assignmentID', {
    templateUrl: '/assets/pages/assignment/' + role + '/' + role +
        'Assignment.html',
    controller: capRole + 'AssignmentCtrl'
  })
  .when('/courses/:courseID/assignments/:assignmentID/' +
        'submissions/:submissionID',
  {
    templateUrl: '/assets/pages/submission/' + role + '/' + role +
        'Submission.html',
    controller: capRole + 'SubmissionCtrl'
  })
  .otherwise({
    templateUrl: '/assets/errorPages/404/404.html',
    controller: '404Ctrl'
  });
}).run(['session', function(session) {
    session.checkAuthenticated(function() {
        // no-op, success, continue to site
    }, function() {
        session.authenticate();
    });
}]);
