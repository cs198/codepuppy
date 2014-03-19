angular.module('codepuppy').controller('NavigationCtrl',
['$scope', '$stateParams', '$http', function($scope, $stateParams, $http) {

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
    $http({method: 'GET', url: '/courses/active.json', cache:true})
    .success(function(data, status, headers, config) {
      var courses = [];
      for (var i = 0; i < data.length; ++i) {
        courses.push({
          name: data[i].course_name + ' (' +
            data[i].term_name + ' ' +
            data[i].period + ')',
          url: '/#/courses/' + data[i].id
        });
      }
      var courseNav = {'title': 'Courses', 'elements': courses,
        'selected': null};
      if ($stateParams.courseID !== undefined) {
        computeSelected(courseNav, $stateParams.courseID);
      }
      $scope.navigationSelectors[0] = courseNav;
    });
  };

  /* COURSE HANDLING */
  getCourses();

  // If they haven't selected a course, this is all we need to render
  if ($stateParams.courseID === undefined)
    return;

  /* ASSIGNMENT HANDLING */
  var getAssignments = function(courseID) {
    $http({method: 'GET', url: '/courses/' + courseID + '/assignments.json', cache:true})
    .success(function(data, status, headers, config) {
      var assignments = [];
      for (var i = 0; i < data.length; ++i) {
        assignments.push({
          'name': 'Assignment ' + data[i].number,
          'url': '/#/courses/' + courseID + '/assignments/' + data[i].id
        });
      }
      var assignmentNav = {'title': 'Assignments', 'elements': assignments,
        'selected': null};
      if ($stateParams.assigmentID !== undefined)
        computeSelected(assignmentNav, $stateParams.assignmentID);
      $scope.navigationSelectors[1] = assignmentNav;
    });
  };

  getAssignments($stateParams.courseID);

  // If they haven't selected an assignment, this is all we need to render
  if ($stateParams.assignmentID === undefined)
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
          'url': '/#/courses/' + courseID + '/assignments/' + assignmentID +
            '/submissions/' + data[i].submission.id
        });
      }
      var submissionNav = {'title': 'Submissions', 'elements': submissions,
        'selected': null};
      if ($stateParams.submissionID !== undefined)
        computeSelected(submissionNav, $stateParams.submissionID);
      $scope.navigationSelectors[2] = submissionNav;
    });
  };

  getSubmissions($stateParams.courseID, $stateParams.assignmentID);
}]);
