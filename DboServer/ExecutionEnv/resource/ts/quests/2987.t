CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2987;
	title = 298702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 298707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 57;
			}
			CDboTSClickNPC
			{
				npcidx = "3148107;";
			}
			CDboTSCheckClrQst
			{
				and = "2986;";
				flink = 0;
				flinknextqid = "2988;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 298714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 298701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4952105;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 298708;
			gtype = 1;
			oklnk = 2;
			area = 298701;
			goal = 298704;
			sort = 298705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 298702;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 298709;
				ctype = 1;
				idx = 3148107;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 298708;
				area = 298701;
				goal = 298704;
				m0fz = "-3911.649902";
				m0widx = 1;
				scitem = -1;
				sort = 298705;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "4937.810059";
				m0ttip = 298715;
				rwd = 100;
				taid = 1;
				title = 298702;
				gtype = 1;
			}
		}
	}
}

