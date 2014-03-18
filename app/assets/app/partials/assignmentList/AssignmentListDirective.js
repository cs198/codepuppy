angular.module('codepuppy').directive('assignmentList', function() {
  return {
    restrict: 'E',
    scope: {
		'assignments': '=assignments'
    },
    templateUrl: '/assets/partials/assignmentList/assignmentList.html'
  };
});
