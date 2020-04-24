CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2970;
	title = 297002;

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
				tblidx = 297020;
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
				widx = 1;
				cct = 1;
				crt = 0;
				r = "30.000000";
				x = "3473.000000";
				z = "-4595.000000";
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 297007;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 55;
			}
			CDboTSClickNPC
			{
				npcidx = "4531104;";
			}
			CDboTSCheckClrQst
			{
				and = "2962;";
				flink = 1;
				flinknextqid = "2971;2972;";
				not = 0;
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
				conv = 297009;
				ctype = 1;
				idx = 4531104;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 297008;
				area = 297001;
				goal = 297004;
				m0fz = "-4595.000000";
				m0widx = 1;
				scitem = -1;
				sort = 297005;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "3473.000000";
				m0ttip = 297015;
				rwd = 100;
				taid = 1;
				title = 297002;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 297014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 297001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4531104;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 297008;
			gtype = 1;
			oklnk = 2;
			area = 297001;
			goal = 297004;
			sort = 297005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 297002;
		}
	}
}

