CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3029;
	title = 302902;

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
			stdiag = 302907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "3183121;";
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 59;
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "3030;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 302914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 302901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4953102;";
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
			cont = 302908;
			gtype = 1;
			oklnk = 2;
			area = 302901;
			goal = 302904;
			sort = 302905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 302902;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 302909;
				ctype = 1;
				idx = 3183121;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 302908;
				area = 302901;
				goal = 302904;
				m0fz = "-5026.399902";
				m0widx = 1;
				scitem = -1;
				sort = 302905;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "3410.469971";
				m0ttip = 302915;
				rwd = 100;
				taid = 1;
				title = 302902;
				gtype = 1;
			}
		}
	}
}

