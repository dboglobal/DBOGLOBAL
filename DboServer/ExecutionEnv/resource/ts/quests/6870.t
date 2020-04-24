CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6870;
	title = 687002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 687007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 15;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 687008;
				gtype = 1;
				area = 687001;
				goal = 687004;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 687005;
				stype = 64;
				taid = 1;
				title = 687002;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 687014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 687001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3185101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 687008;
			gtype = 1;
			oklnk = 2;
			area = 687001;
			goal = 687004;
			sort = 687005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 687002;
		}
	}
}

