CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3012;
	title = 301202;

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
			stdiag = 301207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSClickNPC
			{
				npcidx = "4751414;";
			}
			CDboTSCheckClrQst
			{
				and = "3011;";
				flink = 0;
				flinknextqid = "3013;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 301214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 301201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1403201;";
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
			cont = 301208;
			gtype = 2;
			oklnk = 2;
			area = 301201;
			goal = 301204;
			sort = 301205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 301202;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 301209;
				ctype = 1;
				idx = 4751414;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 301208;
				area = 301201;
				goal = 301204;
				m0fz = "-311.750000";
				m0widx = 6;
				scitem = -1;
				sort = 301205;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "1073.239990";
				m0ttip = 301215;
				rwd = 100;
				taid = 1;
				title = 301202;
				gtype = 2;
			}
		}
	}
}

