CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6153;
	title = 1100628;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 254;
			rm = 0;
			yeslnk = 5;

			CDboTSCheckClrQst
			{
				and = "6153;";
				flink = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 1100630;
				gtype = -1;
				area = 1100802;
				goal = 1100629;
				m0fz = "-3360.000000";
				m0widx = 1;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "6323.000000";
				m0ttip = 1100628;
				rwd = 5;
				taid = 1;
				title = 1100628;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 5;
			desc = 1100763;
			nextlnk = 255;
			rwdtbl = 1160601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100626;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 4;
			}
			CDboTSClickNPC
			{
				npcidx = 4151209;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 1100627;
			gtype = 2;
			oklnk = 2;
			area = 1100802;
			goal = 1100629;
			sort = 105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 5;
			title = 1100628;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSRcvSvrEvt
			{
				id = 16220;
			}
		}
	}
}

