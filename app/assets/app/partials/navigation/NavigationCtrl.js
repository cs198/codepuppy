angular.module('codepuppy').controller('NavigationCtrl', ['$scope', '$routeParams', function($scope, $routeParams) {
  $scope.navigationSelectors = [];
  
  /* Compute the selected attribute for 'navSelector' based on the routing
   * parameter 'targetSuffix' */
  function computeSelected(navSelector, targetSuffix) {
    var endsWith = function (string, query) {
      return string.length >= query.length && 
             string.substr(string.length - query.length) == query;
    };

    var elements = navSelector['elements'];
    for (i = 0; i < elements.length; ++i)
      if (endsWith(elements[i]['url'], targetSuffix))
        navSelector['selected'] = elements[i]['name'];
  }
  
  /* COURSE HANDLING */
  var courses = [{'name': 'CS 106A', 'url': '/#/cs106a'}, 
                 {'name': 'CS 106B', 'url': '/#/cs106b'}, 
                 {'name': 'CS 142' , 'url': '/#/cs142'}
                ]; // TODO: Make API call
  var courseNav = {'title': 'Courses', 'elements': courses, 'selected': null};
  $scope.navigationSelectors.push(courseNav);
  // If they haven't selected a course, this is all we need to render
  if ($routeParams.courseID == null)
    return;
  /* Find which course should be selected */
  computeSelected($scope.navigationSelectors[0], $routeParams.courseID);

  /* ASSIGNMENT HANDLING */
  var assignments = [{'name': 'Assignment One (Game of Life)', 'url': '/#/cs106b/1'}, 
                     {'name': 'Assignment Two (ADTs)', 'url': '/#/cs106b/3'}, 
                     {'name': 'Assignment Three (Recursion)' , 'url': '/#/cs106b/7'}
                    ]; // TODO: Make API call
  var assignmentsNav = {'title': 'Assignments', 'elements': assignments, 'selected': null};
  $scope.navigationSelectors.push(assignmentsNav);
  // If they haven't selected an assignment, this is all we need to render
  if ($routeParams.assignmentID == null)
    return;
  // Find which assignment should be selected
  computeSelected($scope.navigationSelectors[1], $routeParams.assignmentID);


  /* SUBMISSION HANDLING */
  var submissions = [{'name': 'Reid Watson (1)', 'url': '/#/cs106b/3/rawatson_1'}, 
                     {'name': 'Reid Watson (2)', 'url': '/#/cs106b/3/rawatson_2'}, 
                     {'name': 'Maesen Churchill (1)' , 'url': '/#/cs106b/3/maesenc_1'}
                    ]; // TODO: Make API call
  var submissionsNav = {'title': 'Submissions', 'elements': submissions, 'selected': null};
  $scope.navigationSelectors.push(submissionsNav);
  // If they haven't selected an assignment, we're done
  if ($routeParams.submissionID == null)
    return;
  // Find which submission should be selected
  computeSelected($scope.navigationSelectors[2], $routeParams.submissionID);
}]);
