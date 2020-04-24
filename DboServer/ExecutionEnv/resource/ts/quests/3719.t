CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3719;
	title = 371902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 371909;
				ctype = 1;
				idx = 1361101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 371908;
				gtype = 1;
				area = 371901;
				goal = 371904;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 371905;
				stype = 2;
				taid = 1;
				title = 371902;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 371914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 371901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7074102;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 371908;
			gtype = 1;
			oklnk = 2;
			area = 371901;
			goal = 371904;
			sort = 371905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 371902;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 371907;
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
				npcidx = "1361101;";
			}
			CDboTSCheckClrQst
			{
				and = "3718;";
				flink = 0;
				flinknextqid = "3720;";
				not = 0;
			}
		}
	}
}

