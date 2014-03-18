angular.module('codepuppy').controller('LeaderHomeCtrl', ['$scope', function($scope) {
    $http({method: 'GET', url: '/courses/active.json', cache:true})
    .success(function(data, status, headers, config) {
		if(data.length > 0) {
			var path = '/courses/' + data[0].id;
			$location.path(path);
		}
    });
}]);
