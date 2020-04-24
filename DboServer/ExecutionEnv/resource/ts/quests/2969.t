CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2969;
	title = 296902;

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
			stdiag = 296907;
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
				npcidx = "4511901;";
			}
			CDboTSCheckClrQst
			{
				and = "2974;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 296914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 296901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4755101;";
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
			cont = 296908;
			gtype = 1;
			oklnk = 2;
			area = 296901;
			goal = 296904;
			sort = 296905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 296902;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 296909;
				ctype = 1;
				idx = 4511901;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 296908;
				m1fx = "4442.129883";
				area = 296901;
				goal = 296904;
				m1fy = "0.000000";
				m1ttip = 296915;
				scitem = -1;
				sort = 296905;
				stype = 2;
				m1fz = "-3705.649902";
				grade = 132203;
				m1pat = 0;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 296902;
				gtype = 1;
			}
		}
	}
}

