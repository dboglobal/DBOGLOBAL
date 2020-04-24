CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 8;
	rq = 0;
	tid = 2397;
	title = 239702;

	CNtlTSGroup
	{
		gid = 0;

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
			cont = 239708;
			gtype = 1;
			oklnk = 2;
			area = 239701;
			goal = 239704;
			sort = 239705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 239702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 239714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 239701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1651110;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 239707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 52;
			}
			CDboTSClickNPC
			{
				npcidx = "6412111;";
			}
			CDboTSCheckClrQst
			{
				and = "2395;2396;";
				flink = 1;
				flinknextqid = "2398;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 239709;
				ctype = 1;
				idx = 6412111;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 239708;
				area = 239701;
				goal = 239704;
				m0fz = "-1064.010010";
				m0widx = 14;
				scitem = -1;
				sort = 239705;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "949.237976";
				m0ttip = 239715;
				rwd = 100;
				taid = 1;
				title = 239702;
				gtype = 1;
			}
		}
	}
}

