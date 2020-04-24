CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 544;
	title = 54402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 54409;
				ctype = 1;
				idx = 1651104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 54408;
				gtype = 1;
				area = 54401;
				goal = 54404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 54405;
				stype = 1;
				taid = 1;
				title = 54402;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 54407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 19;
			}
			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSClickNPC
			{
				npcidx = "1651104;";
			}
			CDboTSCheckClrQst
			{
				and = "514;";
				flink = 1;
				flinknextqid = "562;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 54408;
			gtype = 1;
			oklnk = 2;
			area = 54401;
			goal = 54404;
			sort = 54405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 54402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 54414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 54401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142109;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

