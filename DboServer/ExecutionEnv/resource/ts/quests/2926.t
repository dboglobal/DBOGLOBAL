CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2926;
	title = 292602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 292607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSClickNPC
			{
				npcidx = "5441401;";
			}
			CDboTSCheckClrQst
			{
				and = "2925;";
				flink = 0;
				flinknextqid = "2927;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 292614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 292601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5042105;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 292608;
			gtype = 1;
			oklnk = 2;
			area = 292601;
			goal = 292604;
			sort = 292605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 292602;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 292609;
				ctype = 1;
				idx = 5441401;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 292608;
				area = 292601;
				goal = 292604;
				m0fz = "-3850.750000";
				m0widx = 1;
				scitem = -1;
				sort = 292605;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "2687.639893";
				m0ttip = 292615;
				rwd = 100;
				taid = 1;
				title = 292602;
				gtype = 1;
			}
		}
	}
}

