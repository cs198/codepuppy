angular.module('codepuppy').controller('NavigationCtrl',
['$scope', '$routeParams', '$http', function($scope, $routeParams, $http) {

  $scope.navigationSelectors = [];

  /* Compute the selected attribute for 'navSelector' based on the routing
   * parameter 'targetSuffix' */
  function computeSelected(navSelector, targetSuffix) {
    var endsWith = function(string, query) {
      return (string.length >= query.length &&
        string.substr(string.length - query.length) == query);
    };

    var elements = navSelector.elements;
    for (i = 0; i < elements.length; ++i) {
      if (endsWith(elements[i].url, targetSuffix)) {
        navSelector.selected = elements[i].name;
      }
    }
  }

  var getCourses = function() {
    $http({method: 'GET', url: '/courses/active.json'})
    .success(function(data, status, headers, config) {
      var courses = [];
      for (var i = 0; i < data.length; ++i) {
        courses.push({
          'name': data[i].course_name + ' (' +
            data[i].term_name + ' ' +
            data[i].period + ')',
          'url': '/#/' + data[i].id
        });
      }
      var courseNav = {'title': 'Courses', 'elements': courses,
        'selected': null};
      if ($routeParams.courseID !== undefined) {
        computeSelected(courseNav, $routeParams.courseID);
      }
      $scope.navigationSelectors[0] = courseNav;
    });
  };

  /* COURSE HANDLING */
  getCourses();

  // If they haven't selected a course, this is all we need to render
  if ($routeParams.courseID === undefined)
    return;

  /* ASSIGNMENT HANDLING */
  var getAssignments = function(courseID) {
    $http({method: 'GET', url: '/courses/' + courseID + '/assignments.json'})
    .success(function(data, status, headers, config) {
      var assignments = [];
      for (var i = 0; i < data.length; ++i) {
        assignments.push({
          'name': 'Assignment ' + data[i].number,
          'url': '/#/' + courseID + '/' + data[i].id
        });
      }
      var assignmentNav = {'title': 'Assignments', 'elements': assignments,
        'selected': null};
      if ($routeParams.assigmentID !== undefined)
        computeSelected(assignmentNav, $routeParams.assignmentID);
      $scope.navigationSelectors[1] = assignmentNav;
    });
  };

  getAssignments($routeParams.courseID);

  // If they haven't selected an assignment, this is all we need to render
  if ($routeParams.assignmentID === undefined)
    return;

  /* SUBMISSION HANDLING */
  var getSubmissions = function(courseID, assignmentID) {
    $http({method: 'GET', url: '/assignments/' + assignmentID +
          '/submissions.json'})
    .success(function(data, status, headers, config) {
      var submissions = [];
      for (var i = 0; i < data.length; ++i) {
        submissions.push({
          'name': 'Submission: ' + data[i].person.user_system_id,
          'url': '/#/' + courseID + '/' + assignmentID + '/' +
            data[i].person.id
        });
      }
      var submissionNav = {'title': 'Submissions', 'elements': submissions,
        'selected': null};
      if ($routeParams.submissionID !== undefined)
        computeSelected(submissionNav, $routeParams.submissionID);
      $scope.navigationSelectors[2] = submissionNav;
    });
  };

  getSubmissions($routeParams.courseID, $routeParams.assignmentID);
}]);
