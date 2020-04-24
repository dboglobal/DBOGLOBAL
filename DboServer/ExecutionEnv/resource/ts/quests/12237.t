CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12237;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActBroadMsg
			{
				dt = 5;
				et = 2;
				oidx = 7511101;
				oqtidx = 1101095;
				ot = 0;
				uisdt = 2;
				oc = 0;
				taid = 1;
				uisht = 1;
				uibst = 0;
				uidt = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 1078;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActSendSvrEvt
			{
				id = 1079;
				tblidx = -1;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 1;
		}
	}
}

