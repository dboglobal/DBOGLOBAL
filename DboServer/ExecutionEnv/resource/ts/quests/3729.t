CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3729;
	title = 372902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 372907;
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
				npcidx = "3204301;";
			}
			CDboTSCheckClrQst
			{
				and = "3728;";
				flink = 0;
				flinknextqid = "3730;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 372909;
				ctype = 1;
				idx = 3204301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 372908;
				gtype = 1;
				area = 372901;
				goal = 372904;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 372905;
				stype = 2;
				taid = 1;
				title = 372902;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 372914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 372901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3204101;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 372908;
			gtype = 1;
			oklnk = 2;
			area = 372901;
			goal = 372904;
			sort = 372905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 372902;
		}
	}
}

