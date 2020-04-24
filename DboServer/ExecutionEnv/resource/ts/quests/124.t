CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 124;
	title = 12402;

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
				conv = 12409;
				ctype = 1;
				idx = 5062103;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 12408;
				area = 12401;
				goal = 12404;
				m0fz = "3102.379883";
				m0widx = 1;
				scitem = -1;
				sort = 12405;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "5818.370117";
				m0ttip = 12415;
				rwd = 100;
				taid = 1;
				title = 12402;
				gtype = 1;
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
			stdiag = 12407;
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
				minlvl = 17;
			}
			CDboTSClickNPC
			{
				npcidx = "5062103;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "125;";
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 12408;
			gtype = 1;
			oklnk = 2;
			area = 12401;
			goal = 12404;
			sort = 12405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 12402;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 12414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 12401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4372202;";
			}
		}
	}
}

