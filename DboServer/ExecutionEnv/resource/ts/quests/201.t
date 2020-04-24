CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 201;
	title = 20102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActOutMsg
			{
				idx = 14;
				taid = 1;
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 100;
			cid = 6;
			idx = 2013;
			lilnk = 255;
			oklnk = 100;
			prelnk = "5;";
		}
		CDboTSContGCond
		{
			cid = 5;
			prelnk = "7;100;";
			nolnk = 255;
			rm = 1;
			yeslnk = 6;

			CDboTSClickNPC
			{
				npcidx = "4511205;";
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 1;
			cid = 4;
			idx = 2011;
			lilnk = 255;
			oklnk = 1;
			prelnk = "0;";
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 7;

			CDboTSScoutUse
			{
				idx = 60000000;
				tidx = 4511205;
				tt = 1;
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
			prelnk = "1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 20107;
			nolnk = 253;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "4511205;";
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 1;
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "202;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 20109;
				ctype = 1;
				idx = 4511205;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 20108;
				area = 20101;
				goal = 20104;
				m0fz = "-2361.800049";
				m0widx = 1;
				scitem = -1;
				sort = 20105;
				stype = 64;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "2892.610107";
				m0ttip = 20115;
				rwd = 100;
				taid = 1;
				title = 20102;
				gtype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 5;
			cid = 100;
			rwdzeny = 0;
			desc = 20114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 20101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "6;6;";
			usetbl = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 20108;
			gtype = 1;
			oklnk = 2;
			area = 20101;
			goal = 20104;
			sort = 20105;
			prelnk = "4;4;";
			ds = 0;
			grade = 132203;
			rwd = 100;
			title = 20102;
		}
	}
}

