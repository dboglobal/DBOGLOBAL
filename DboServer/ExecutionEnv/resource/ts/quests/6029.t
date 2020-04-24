CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6029;
	title = 602902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 602909;
				ctype = 1;
				idx = 4511410;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 602908;
				gtype = 1;
				area = 602901;
				goal = 602904;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 602905;
				stype = 8;
				taid = 1;
				title = 602902;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 254;
			cid = 1;
			cont = 602908;
			gtype = 1;
			oklnk = 2;
			area = 602901;
			goal = 602904;
			sort = 602905;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 602902;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "0;1;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 602907;
			nolnk = 254;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = "4511410;";
			}
			CDboTSCheckClrQst
			{
				and = "6028;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 602914;
			nextlnk = 253;
			rwdexp = 0;
			rwdtbl = 602901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4512102;";
			}
		}
	}
}

