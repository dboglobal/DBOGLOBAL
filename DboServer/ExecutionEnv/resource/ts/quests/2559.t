CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2559;
	title = 255902;

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
				conv = 255909;
				ctype = 1;
				idx = 4751207;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 255908;
				gtype = 1;
				area = 255901;
				goal = 255904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 255905;
				stype = 2;
				taid = 1;
				title = 255902;
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
			desc = 255914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 255901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "5101201;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 255908;
			gtype = 1;
			oklnk = 2;
			area = 255901;
			goal = 255904;
			sort = 255905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 255902;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 255907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSCheckClrQst
			{
				and = "2558;";
				flink = 1;
				flinknextqid = "2560;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4751207;";
			}
		}
	}
}

