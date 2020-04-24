CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3777;
	title = 377702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 377720;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSColRgn
			{
				widx = 15;
				cct = 1;
				crt = 0;
				r = "30.000000";
				x = "-4647.870117";
				z = "-5457.109863";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 377709;
				ctype = 1;
				idx = 5631101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 377708;
				area = 377701;
				goal = 377704;
				m0fz = "-5457.169922";
				m0widx = 15;
				scitem = -1;
				sort = 377705;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "-4647.870117";
				m0ttip = 377715;
				rwd = 100;
				taid = 1;
				title = 377702;
				gtype = 1;
			}
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
			desc = 377714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 377701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5631101;";
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
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 377707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "5631101;";
			}
			CDboTSCheckClrQst
			{
				and = "3774;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 377708;
			gtype = 1;
			oklnk = 2;
			area = 377701;
			goal = 377704;
			sort = 377705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 377702;
		}
	}
}

